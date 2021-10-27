#pragma once

#include <QtWidgets/QWidget>
#include <qtextedit.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include "ui_TextAutoComplete.h"
#include "TrieDictionary.h"

class TextAutoComplete : public QWidget
{
    Q_OBJECT

public:
    TextAutoComplete(QWidget *parent = Q_NULLPTR);
    void suggestionSelected(QPushButton* S);
    void changeSuggestion();

private:
    Ui::TextAutoCompleteClass ui;
    Trie* trie;
    QTextEdit* text_box;
    QLayout* main_layout;
    QLabel* dict_size;
    QPushButton* suggestion1;
    QPushButton* suggestion2;
    QPushButton* suggestion3;
};
