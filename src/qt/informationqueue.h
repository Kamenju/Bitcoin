// This file is part of Bitcoin.
// Copyright (C) 2015 Tom Zander
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see
// http://opensource.org/licenses/gpl-3.0.html

#ifndef INFORMATIONQUEUE_H
#define INFORMATIONQUEUE_H

#include <QObject>
#include <QPointer>
#include <QStringList>
#include <QScrollArea>

#include <forms/ui_informationqueuedialog.h>
#include <forms/ui_usermessagewidget.h>

class QWidget;

struct UserMessage {
    UserMessage() : flags(0) {}
    QString title;
    QString message;
    unsigned int flags;  // flags from CClientUIInterface::MessageBoxFlags (ui_interface.h)
};

class InformationQueue;
class InformationDialog;

class UserMessageWidget : public QWidget
{
    Q_OBJECT
public:
    UserMessageWidget(InformationQueue *queue, int messageId);

    void refresh();

    int messageId() const {
        return m_messageId;
    }

    unsigned int flags() const;

private:
    Ui::UserMessageWidget m_widget;
    InformationQueue *m_parent;
    int m_messageId;
};

/**
 * this class gathers up all the messages that are send by different
 * components to be shown to the user.  The queue may get long if the
 * use doesn't respond, this class takes care that it doesn't grow
 * out of proportion.   For instance by merging duplicate messages.
 */
class InformationQueue : public QObject
{
    Q_OBJECT
public:
    InformationQueue(QWidget *parent = 0);

    void removeMessages(const QSet<int> &messageIds);

public slots:
    void addMessage(const UserMessage &message);
    void hideDialog();

private slots:
    void updateUI();

private:
    struct Message {
        QString title;
        QStringList messages;
        unsigned int flags;
        QPointer<UserMessageWidget> widget;
        int id;
    };
    friend class UserMessageWidget;

    QWidget *m_parent;
    InformationDialog *m_infoDialog;

    QList<Message> m_messageQueue;
    bool m_updateScheduled;
    int m_lastMessageId;
};

class InformationDialog : public QDialog
{
    Q_OBJECT
public:
    InformationDialog(InformationQueue *queue, QWidget *parent = 0);

    void addWidget(UserMessageWidget *widget);

private slots:
    void closePressed();

private:
    Ui::FeedbackDialog m_widget;
    InformationQueue *m_queue;
    QSet<int> m_messages;

    QScrollArea *m_infoPage;
    QScrollArea *m_warningPage;
    QScrollArea *m_errorPage;
};


#endif
