/***************************************************************************
**
** This file is part of Qt Creator
**
** Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).
**
** Contact:  Qt Software Information (qt-info@nokia.com)
**
**
** Non-Open Source Usage
**
** Licensees may use this file in accordance with the Qt Beta Version
** License Agreement, Agreement version 2.2 provided with the Software or,
** alternatively, in accordance with the terms contained in a written
** agreement between you and Nokia.
**
** GNU General Public License Usage
**
** Alternatively, this file may be used under the terms of the GNU General
** Public License versions 2.0 or 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the packaging
** of this file.  Please review the following information to ensure GNU
** General Public Licensing requirements will be met:
**
** http://www.fsf.org/licensing/licenses/info/GPLv2.html and
** http://www.gnu.org/copyleft/gpl.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights. These rights are described in the Nokia Qt GPL Exception
** version 1.3, included in the file GPL_EXCEPTION.txt in this package.
**
***************************************************************************/

#include "handler.h"

#include <QtCore/QDebug>
#include <QtCore/QObject>
#include <QtCore/QRegExp>
#include <QtCore/QStack>

#include <QtGui/QKeyEvent>
#include <QtGui/QLineEdit>
#include <QtGui/QPlainTextEdit>
#include <QtGui/QScrollBar>
#include <QtGui/QTextBlock>
#include <QtGui/QTextCursor>
#include <QtGui/QTextEdit>


using namespace FakeVim::Internal;

#define StartOfLine QTextCursor::StartOfLine
#define EndOfLine QTextCursor::EndOfLine
#define MoveAnchor QTextCursor::MoveAnchor
#define KeepAnchor QTextCursor::KeepAnchor
#define Up QTextCursor::Up
#define Down QTextCursor::Down
#define Right QTextCursor::Right
#define Left QTextCursor::Left


///////////////////////////////////////////////////////////////////////
//
// FakeVimHandler
//
///////////////////////////////////////////////////////////////////////

const int ParagraphSeparator = 0x00002029;

using namespace Qt;

enum Mode
{
    InsertMode,
    CommandMode,
    ExMode
};

enum SubMode
{
    NoSubMode,
    RegisterSubMode,
    ChangeSubMode,
    DeleteSubMode,
    ZSubMode
};

class FakeVimHandler::Private
{
public:
    Private(FakeVimHandler *parent);

    bool eventFilter(QObject *ob, QEvent *ev);

    static int shift(int key) { return key + 32; }
    static int control(int key) { return key + 256; }

    void init();
    void handleKey(int key, const QString &text);
    void handleInsertMode(int key, const QString &text);
    void handleCommandMode(int key, const QString &text);
    void handleRegisterMode(int key, const QString &text);
    void handleExMode(int key, const QString &text);
    void finishMovement();
    void updateCommandBuffer();
    void search(const QString &needle, bool backwards);
    void showMessage(const QString &msg);

    int mvCount() const { return m_mvcount.isEmpty() ? 1 : m_mvcount.toInt(); }
    int opCount() const { return m_opcount.isEmpty() ? 1 : m_opcount.toInt(); }
    int count() const { return mvCount() * opCount(); }
    int leftDist() const { return m_tc.position() - m_tc.block().position(); }
    int rightDist() const { return m_tc.block().length() - leftDist() - 1; }
    bool atEol() const { return m_tc.atBlockEnd() && m_tc.block().length()>1; }

    int lastPositionInDocument() const;

	// all zero-based counting
	int cursorLineOnScreen() const;
	int linesOnScreen() const;
	int cursorLineInDocument() const;
	void scrollToLineInDocument(int line);

    void moveToFirstNonBlankOnLine();
    void moveToNextWord(int repeat, bool simple);
    void moveToWordBegin(int repeat, bool simple);
    void moveToWordEnd(int repeat, bool simple);

    FakeVimHandler *q;
    Mode m_mode;
    SubMode m_submode;
    QString m_input;
    QPlainTextEdit *m_editor;
    QTextCursor m_tc;
    QHash<int, QString> m_registers;
    int m_register;
    QString m_mvcount;
    QString m_opcount;

    QStack<QString> m_undoStack;
    QStack<QString> m_redoStack;

    bool m_fakeEnd;

    int m_commandCode; // ?, /, : ...
    QString m_commandBuffer;

    bool m_lastSearchBackward;
    QString m_lastSearchString;
};

FakeVimHandler::Private::Private(FakeVimHandler *parent)
{
    q = parent;
    m_mode = CommandMode;
    m_fakeEnd = false;
    m_lastSearchBackward = false;
    m_register = '"';
}

bool FakeVimHandler::Private::eventFilter(QObject *ob, QEvent *ev)
{
    QKeyEvent *keyEvent = static_cast<QKeyEvent *>(ev);
    int key = keyEvent->key();
    if (key == Key_Shift || key == Key_Alt || key == Key_Control
        || key == Key_Alt || key == Key_AltGr || key == Key_Meta)
        return false;
    //qDebug() << "KEY: " << key << Qt::ShiftModifier;

    // Fake "End of line"
    m_editor = qobject_cast<QPlainTextEdit *>(ob);
    if (!m_editor)
        return false;

    m_tc = m_editor->textCursor();
    if (m_fakeEnd) {
        //m_fakeEnd = false;
        m_tc.movePosition(Right, MoveAnchor, 1);
        qDebug() << "Unfake EOL";
    }

    if (key >= Key_A && key <= Key_Z
        && (keyEvent->modifiers() & Qt::ShiftModifier) == 0)
        key += 32;
    if ((keyEvent->modifiers() & Qt::ControlModifier) != 0)
        key += 256;
    handleKey(key, keyEvent->text());

    // We fake vi-style end-of-line behaviour
    m_fakeEnd = atEol() && m_mode == CommandMode;

    //qDebug() << "POS: " <<  m_tc.position()
    //    << "  BLOCK LEN: " << m_tc.block().length()
    //    << "  LEFT: " << leftDist() << " RIGHT: " << rightDist();

    if (m_fakeEnd) {
        m_tc.movePosition(Left, MoveAnchor, 1);
        qDebug() << "Fake EOL";
    }

    //qDebug() << "POS: " <<  m_tc.position()
    //    << "  BLOCK LEN: " << m_tc.block().length()
    //    << "  LEFT: " << leftDist() << " RIGHT: " << rightDist();

    m_editor->setTextCursor(m_tc);
    m_editor->ensureCursorVisible();
    return true;
}

void FakeVimHandler::Private::handleKey(int key, const QString &text)
{
    if (m_mode == InsertMode)
        handleInsertMode(key, text);
    else if (m_mode == CommandMode)
        handleCommandMode(key, text);
    else if (m_mode == ExMode)
        handleExMode(key, text);
}

void FakeVimHandler::Private::finishMovement()
{
    if (m_submode == ChangeSubMode) {
        m_registers[m_register] = m_tc.selectedText();
        m_tc.removeSelectedText();
        m_mode = InsertMode;
        m_submode = NoSubMode;
    } else if (m_submode == DeleteSubMode) {
        m_registers[m_register] = m_tc.selectedText();
        m_tc.removeSelectedText();
        m_submode = NoSubMode;
        if (atEol())
            m_tc.movePosition(Left, MoveAnchor, 1);
    }
    m_mvcount.clear();
    m_opcount.clear();
    m_register = '"';
    m_tc.clearSelection();
    updateCommandBuffer();
}

void FakeVimHandler::Private::updateCommandBuffer()
{
    //qDebug() << "CMD" << m_commandBuffer;
    QString msg = QChar(m_commandCode ? m_commandCode : ' ') + m_commandBuffer;
    emit q->commandBufferChanged(msg);
}

void FakeVimHandler::Private::showMessage(const QString &msg)
{
    emit q->commandBufferChanged(msg);
}

void FakeVimHandler::Private::handleCommandMode(int key, const QString &text)
{
    Q_UNUSED(text)

    //qDebug() << "-> MODE: " << m_mode << " KEY: " << key;
    if (m_submode == RegisterSubMode) {
        m_register = key;
        m_submode = NoSubMode;
    } else if (m_submode == ChangeSubMode && key == 'c') {
        m_tc.movePosition(StartOfLine, MoveAnchor);
        m_tc.movePosition(Down, KeepAnchor, count());
        m_registers[m_register] = m_tc.selectedText();
        finishMovement();
    } else if (m_submode == DeleteSubMode && key == 'd') {
        m_tc.movePosition(StartOfLine, MoveAnchor);
        m_tc.movePosition(Down, KeepAnchor, count());
        m_registers[m_register] = m_tc.selectedText();
        finishMovement();
    } else if (m_submode == ZSubMode) {
        if (key == Key_Return) {
            // cursor line to top of window, cursor on first non-blank
			scrollToLineInDocument(cursorLineInDocument());
            moveToFirstNonBlankOnLine();
            finishMovement();
        } else {
            qDebug() << "Ignored z + " << key << text;
        }
        m_submode = NoSubMode;
    } else if (key >= '0' && key <= '9') {
        if (key == '0' && m_mvcount.isEmpty()) {
            m_tc.movePosition(StartOfLine, KeepAnchor);
            finishMovement();
        } else {
            m_mvcount.append(QChar(key));
        }
    } else if (key == ':' || key == '/' || key == '?') {
        m_commandCode = key;
        m_mode = ExMode;
        updateCommandBuffer();
    } else if (key == '|') {
        m_tc.movePosition(StartOfLine, KeepAnchor);
        m_tc.movePosition(Right, KeepAnchor, qMin(count(), rightDist()) - 1);
        finishMovement();
    } else if (key == '"') {
        m_submode = RegisterSubMode;
    } else if (key == Key_Return) {
        m_tc.movePosition(StartOfLine);
        m_tc.movePosition(Down);
    } else if (key == Key_Home) {
        m_tc.movePosition(StartOfLine, KeepAnchor);
        finishMovement();
    } else if (key == '$' || key == Key_End) {
        m_tc.movePosition(EndOfLine, KeepAnchor);
        finishMovement();
    } else if (key == 'A') {
        m_tc.movePosition(EndOfLine, MoveAnchor);
        m_mode = InsertMode;
    } else if (key == 'b') {
        moveToWordBegin(count(), false);
        finishMovement();
    } else if (key == 'B') {
        moveToWordBegin(count(), true);
        finishMovement();
    } else if (key == 'c') {
        m_submode = ChangeSubMode;
    } else if (key == 'C') {
        m_submode = ChangeSubMode;
        m_tc.movePosition(EndOfLine, KeepAnchor);
        finishMovement();
    } else if (key == 'd') {
        if (atEol())
            m_tc.movePosition(Left, MoveAnchor, 1);
        m_opcount = m_mvcount;
        m_mvcount.clear();
        m_submode = DeleteSubMode;
    } else if (key == 'D') {
        m_submode = DeleteSubMode;
        m_tc.movePosition(EndOfLine, KeepAnchor);
        finishMovement();
    } else if (key == 'e') {
        moveToWordEnd(count(), false);
        finishMovement();
    } else if (key == 'E') {
        moveToWordEnd(count(), true);
        finishMovement();
    } else if (key == 'h' || key == Key_Left) {
        int n = qMin(count(), leftDist());
        if (m_fakeEnd && m_tc.block().length() > 1)
            ++n;
        m_tc.movePosition(Left, KeepAnchor, n);
        finishMovement();
    } else if (key == 'H') {
        m_tc = m_editor->cursorForPosition(QPoint(0, 0));
        m_tc.movePosition(Down, KeepAnchor, qMax(count() - 1, 0));
        moveToFirstNonBlankOnLine();
        finishMovement();
    } else if (key == 'i') {
        m_mode = InsertMode;
    } else if (key == 'j' || key == Key_Down) {
        m_tc.movePosition(Down, KeepAnchor, count());
        finishMovement();
    } else if (key == 'k' || key == Key_Up) {
        m_tc.movePosition(Up, KeepAnchor, count());
        finishMovement();
    } else if (key == 'l' || key == Key_Right) {
        m_tc.movePosition(Right, KeepAnchor, qMin(count(), rightDist()));
        finishMovement();
    } else if (key == 'L') {
        int heigth = m_editor->height();
        m_tc = m_editor->cursorForPosition(QPoint(0, heigth));
        m_tc.movePosition(Up, KeepAnchor, qMax(count(), 1));
        moveToFirstNonBlankOnLine();
        finishMovement();
    } else if (key == 'M') {
        int heigth = m_editor->height();
        m_tc = m_editor->cursorForPosition(QPoint(0, heigth / 2));
        moveToFirstNonBlankOnLine();
        finishMovement();
    } else if (key == 'n') {
        search(m_lastSearchString, m_lastSearchBackward);
    } else if (key == 'N') {
        search(m_lastSearchString, !m_lastSearchBackward);
    } else if (key == 'p') {
        QString text = m_registers[m_register];
        int n = text.count(QChar(ParagraphSeparator));
        if (n > 0) {
            m_tc.movePosition(Down);
            m_tc.movePosition(StartOfLine);
            m_tc.insertText(text);
            m_tc.movePosition(Up, MoveAnchor, n);
        } else {
            m_tc.movePosition(Right);
            m_tc.insertText(text);
            m_tc.movePosition(Left);
        }
    } else if (key == control('r')) {
        m_editor->redo();
    } else if (key == 'u') {
        m_editor->undo();
    } else if (key == 'w') {
        moveToNextWord(count(), false);
        finishMovement();
    } else if (key == 'W') {
        moveToNextWord(count(), true);
        finishMovement();
    } else if (key == 'x') { // = "dl"
        if (atEol())
            m_tc.movePosition(Left, MoveAnchor, 1);
        m_submode = DeleteSubMode;
        m_tc.movePosition(Right, KeepAnchor, qMin(count(), rightDist()));
        finishMovement();
    } else if (key == 'X') {
        if (leftDist() > 0) {
            m_tc.movePosition(Left, KeepAnchor, qMin(count(), leftDist()));
            m_tc.deleteChar();
        }
        finishMovement();
    } else if (key == 'z') {
        m_submode = ZSubMode;
    } else if (key == '~' && !atEol()) {
        m_tc.movePosition(Right, KeepAnchor, qMin(count(), rightDist()));
        QString str = m_tc.selectedText();
        for (int i = str.size(); --i >= 0; ) {
            QChar c = str.at(i);
            str[i] = c.isUpper() ? c.toLower() : c.toUpper();
        }
        m_tc.deleteChar();
        m_tc.insertText(str);
    } else if (key == Key_PageDown || key == control('f')) {
        m_tc.movePosition(Down, KeepAnchor, count() * (linesOnScreen() - 2));
        finishMovement();
    } else if (key == Key_PageUp || key == control('b')) {
        m_tc.movePosition(Up, KeepAnchor, count() * (linesOnScreen() - 2));
        finishMovement();
    } else if (key == Key_Backspace) {
        m_tc.deletePreviousChar();
    } else if (key == Key_Delete) {
        m_tc.deleteChar();
    } else {
        qDebug() << "Ignored" << key << text;
    }    
}

void FakeVimHandler::Private::handleInsertMode(int key, const QString &text)
{
    if (key == Key_Escape) {
        m_mode = CommandMode;
        m_tc.movePosition(Left, KeepAnchor, qMin(1, leftDist()));
    } else if (key == Key_Left) {
        m_tc.movePosition(Left, MoveAnchor, 1);
    } else if (key == Key_Down) {
        m_tc.movePosition(Down, MoveAnchor, 1);
    } else if (key == Key_Up) {
        m_tc.movePosition(Up, MoveAnchor, 1);
    } else if (key == Key_Right) {
        m_tc.movePosition(Right, MoveAnchor, 1);
    } else if (key == Key_Return) {
        m_tc.insertBlock();
    } else if (key == Key_Backspace) {
        m_tc.deletePreviousChar();
    } else if (key == Key_Delete) {
        m_tc.deleteChar();
    } else if (key == Key_PageDown || key == control('f')) {
        m_tc.movePosition(Down, KeepAnchor, count() * (linesOnScreen() - 2));
        finishMovement();
    } else if (key == Key_PageUp || key == control('b')) {
        m_tc.movePosition(Up, KeepAnchor, count() * (linesOnScreen() - 2));
        finishMovement();
    } else {
        m_tc.insertText(text);
    }    
}

void FakeVimHandler::Private::handleExMode(int key, const QString &text)
{
    Q_UNUSED(text)

    if (key == Key_Escape) {
        m_commandBuffer.clear();
        m_commandCode = 0;
        m_mode = CommandMode;
    } else if (key == Key_Backspace) {
        if (m_commandBuffer.isEmpty())
            m_mode = CommandMode;
        else
            m_commandBuffer.chop(1);
    } else if (key == Key_Return && m_commandCode == ':') {
        static QRegExp reGoto("^(\\d+)$");
        if (reGoto.indexIn(m_commandBuffer) != -1) {
            int n = reGoto.cap(1).toInt();
            m_tc.setPosition(m_tc.block().document()
                ->findBlockByNumber(n - 1).position());
        } else if (m_commandBuffer == "q!" || m_commandBuffer == "q") {
            q->quitRequested(m_editor);
        }
        m_commandBuffer.clear();
        m_mode = CommandMode;
    } else if (key == Key_Return
            && (m_commandCode == '/' || m_commandCode == '?')) {
        m_lastSearchBackward = (m_commandCode == '?');
        m_lastSearchString = m_commandBuffer;
        search(m_lastSearchString, m_lastSearchBackward);
        m_commandBuffer.clear();
        m_commandCode = 0;
        m_mode = CommandMode;
    } else {
        m_commandBuffer += QChar(key);
    }
    updateCommandBuffer();
}

void FakeVimHandler::Private::search(const QString &needle, bool backwards)
{
    //qDebug() << "NEEDLE " << needle << "BACKWARDS" << backwards;
    QTextCursor orig = m_tc;
    QTextDocument::FindFlags flags;
    if (backwards)
        flags = QTextDocument::FindBackward;

    if (!backwards)
        m_tc.movePosition(Right, MoveAnchor, 1);

    m_editor->setTextCursor(m_tc);
    if (m_editor->find(needle, flags)) {
        m_tc = m_editor->textCursor();
        m_tc.movePosition(Left, MoveAnchor, needle.size() - 1);
        return;
    }

    m_tc.setPosition(backwards ? lastPositionInDocument() - 1 : 0);
    m_editor->setTextCursor(m_tc);
    if (m_editor->find(needle, flags)) {
        m_tc = m_editor->textCursor();
        m_tc.movePosition(Left, MoveAnchor, needle.size() - 1);
        if (backwards)
            showMessage("search hit TOP, continuing at BOTTOM");
        else
            showMessage("search hit BOTTOM, continuing at TOP");
        return;
    }

    m_tc = orig;
}

void FakeVimHandler::Private::moveToFirstNonBlankOnLine()
{
    QTextBlock block = m_tc.block();
    QTextDocument *doc = m_tc.document();
    m_tc.movePosition(StartOfLine);
    int firstPos = m_tc.position();
    for (int i = firstPos, n = firstPos + block.length(); i < n; ++i) {
        if (!doc->characterAt(i).isSpace()) {
            m_tc.setPosition(i, KeepAnchor);
            return;
        }
    }
}

static int charClass(QChar c, bool simple)
{
    if (simple)
        return c.isSpace() ? 0 : 1;
    if (c.isLetterOrNumber() || c.unicode() == '_')
        return 2;
    return c.isSpace() ? 0 : 1;
}

void FakeVimHandler::Private::moveToWordBegin(int repeat, bool simple)
{
    QTextDocument *doc = m_tc.document();
    int lastClass = 0;
    while (true) {
        m_tc.movePosition(Left, KeepAnchor, 1);
        QChar c = doc->characterAt(m_tc.position());
        int thisClass = charClass(c, simple);
        if (thisClass != lastClass && lastClass != 0)
            --repeat;
        if (repeat == -1) {
            m_tc.movePosition(Right, KeepAnchor, 1);
            break;
        }
        lastClass = thisClass;
        if (m_tc.position() == 0)
            break;
    }
}

void FakeVimHandler::Private::moveToWordEnd(int repeat, bool simple)
{
    QTextDocument *doc = m_tc.document();
    int n = lastPositionInDocument() - 1;
    int lastClass = 0;
    while (true) {
        m_tc.movePosition(Right, KeepAnchor, 1);
        QChar c = doc->characterAt(m_tc.position());
        int thisClass = charClass(c, simple);
        if (thisClass != lastClass && lastClass != 0)
            --repeat;
        if (repeat == 0) {
            m_tc.movePosition(Left, KeepAnchor, 1);
            break;
        }
        lastClass = thisClass;
        if (m_tc.position() == n)
            break;
    }
}

void FakeVimHandler::Private::moveToNextWord(int repeat, bool simple)
{
    // FIXME: 'w' should stop on empty lines, too
    QTextDocument *doc = m_tc.document();
    int n = lastPositionInDocument() - 1;
    int lastClass = 0;
    while (true) {
        QChar c = doc->characterAt(m_tc.position());
        int thisClass = charClass(c, simple);
        if (thisClass != lastClass && thisClass != 0)
            --repeat;
        if (repeat == -1)
            break;
        lastClass = thisClass;
        m_tc.movePosition(Right, KeepAnchor, 1);
        if (m_tc.position() == n)
            break;
    }
}

int FakeVimHandler::Private::cursorLineOnScreen() const
{
	QRect rect = m_editor->cursorRect();
	return rect.y() / rect.height();
}

int FakeVimHandler::Private::linesOnScreen() const
{
	QRect rect = m_editor->cursorRect();
	//qDebug() <<  m_editor->height() / rect.height();
	return m_editor->height() / rect.height();
}

int FakeVimHandler::Private::cursorLineInDocument() const
{
	//qDebug() << "CURSOR LINE IN DOCUMENT " << m_tc.block().blockNumber();
	return m_tc.block().blockNumber();
}

void FakeVimHandler::Private::scrollToLineInDocument(int line)
{
	// FIXME: works only for QPlainTextEdit
	QScrollBar *scrollBar = m_editor->verticalScrollBar();
	scrollBar->setValue(line);
}

int FakeVimHandler::Private::lastPositionInDocument() const
{
    QTextBlock block = m_tc.block().document()->lastBlock();
    return block.position() + block.length();
}

///////////////////////////////////////////////////////////////////////
//
// FakeVimHandler
//
///////////////////////////////////////////////////////////////////////

FakeVimHandler::FakeVimHandler(QObject *parent)
    : QObject(parent), d(new Private(this))
{}

FakeVimHandler::~FakeVimHandler()
{
    delete d;
}

bool FakeVimHandler::eventFilter(QObject *ob, QEvent *ev)
{
    if (ev->type() != QEvent::KeyPress)
        return QObject::eventFilter(ob, ev);
    return d->eventFilter(ob, ev);
}

