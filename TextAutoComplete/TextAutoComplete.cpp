#include "TextAutoComplete.h"
#include <qlayout.h>
#include <qlineedit.h>
#include <sstream>
TextAutoComplete::TextAutoComplete(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
    this->setWindowTitle("Generic Text Editor");
    trie = new Trie("C:\\Users\\lenovo\\Documents\\DSA-LAB\\Trie\\words", "C:\\Users\\lenovo\\source\\repos\\TextAutoComplete\\TextAutoComplete\\commons.txt");
    trie->loadFromFileCommonWords();
    trie->loadFromFileWords();
    main_layout = new QVBoxLayout();
    this->setLayout(main_layout);
    text_box = new QTextEdit();
    main_layout->addWidget(text_box);
    suggestion1 = new QPushButton();
    suggestion2 = new QPushButton();
    suggestion3 = new QPushButton();
    main_layout->addWidget(suggestion1);
    main_layout->addWidget(suggestion2);
    main_layout->addWidget(suggestion3);
    
    connect(text_box, &QTextEdit::textChanged, this, &TextAutoComplete::changeSuggestion);
    connect(suggestion1, &QPushButton::pressed, this, &TextAutoComplete::);

}

void TextAutoComplete::changeSuggestion(){
    std::stringstream raw_text;
    raw_text << text_box->toPlainText().toStdString();
    if (raw_text.str()[raw_text.str().size() - 1] != ' ') {
        std::vector<std::string> words;
        std::string sample;
        while (raw_text >> sample) {
            words.push_back(sample);
        }
        if (words.size() > 0) {
            sample = words[words.size() - 1];
            std::vector<std::string> suggested = trie->suggestCompletions(sample);
            if (suggested.size() == 1) {
                suggestion1->setText(suggested[0].c_str());
                suggestion2->setText("");
                suggestion3->setText("");
            }
            if (suggested.size() == 2) {
                suggestion1->setText(suggested[0].c_str());
                suggestion2->setText(suggested[1].c_str());
                suggestion3->setText("");
            }
            if (suggested.size() == 3) {
                suggestion1->setText(suggested[0].c_str());
                suggestion2->setText(suggested[1].c_str());
                suggestion3->setText(suggested[2].c_str());
            }
            //suggestion2->setText(std::get<1>(suggested).c_str());
            //suggestion3->setText(std::get<2>(suggested).c_str());
        }

    }
}
