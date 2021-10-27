#include "TextAutoComplete.h"
#include <qlayout.h>
#include <qlineedit.h>
#include <sstream>
TextAutoComplete::TextAutoComplete(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
    this->setWindowTitle("Generic Text Editor");
    trie = new Trie("./words.txt", "./comms_google.txt");
    trie->loadFromFileCommonWords();
    trie->loadFromFileWords();
    dict_size = new QLabel(QString::number(trie->size));
    main_layout = new QVBoxLayout();
    this->setLayout(main_layout);
    text_box = new QTextEdit();
    main_layout->addWidget(text_box);
    suggestion1 = new QPushButton();
    suggestion2 = new QPushButton();
    suggestion3 = new QPushButton();
    main_layout->addWidget(dict_size);
    main_layout->addWidget(suggestion1);
    main_layout->addWidget(suggestion2);
    main_layout->addWidget(suggestion3);
    
    connect(text_box, &QTextEdit::textChanged, this, &TextAutoComplete::changeSuggestion);
    connect(suggestion1, &QPushButton::pressed, this, [this] {suggestionSelected(suggestion1); });
    connect(suggestion2, &QPushButton::pressed, this, [this] {suggestionSelected(suggestion2); });
    connect(suggestion3, &QPushButton::pressed, this, [this] {suggestionSelected(suggestion3); });

}


void TextAutoComplete::suggestionSelected(QPushButton* s){
    std::string buttontext = s->text().toStdString();
    if (buttontext != "") {
        std::stringstream raw_text;
        std::vector<std::string> words;
        std::string sample;
        std::string raw_string = text_box->toPlainText().toStdString();
        raw_text << raw_string;
        if (raw_text.str()[raw_text.str().size() - 1] != ' ') {
            while (raw_text >> sample) {
                words.push_back(sample);
            }

            words.pop_back();
            words.push_back(buttontext);
            std::stringstream new_text;
            for (std::string word : words) {
                new_text << word;
                new_text << " ";
            }

            text_box->setText(new_text.str().c_str());
        }
    }
}


void TextAutoComplete::changeSuggestion(){
    std::stringstream raw_text;
    std::string raw_string = text_box->toPlainText().toStdString();
    raw_text << raw_string;
    bool capitalized = false;
    if (raw_string.size() != 0 && raw_string[raw_string.size() - 1] != ' ') {
        std::vector<std::string> words;
        std::string sample;
        while (raw_text >> sample) {
            words.push_back(sample);
        }
        if (words.size() > 0) {
            sample = words[words.size() - 1]; 

            if (sample[0] >= 'A' && sample[0] <= 'Z') {
                    capitalized = true;
            }

            for (int i = 0; i < sample.size() ; i++) {
                    sample[i] = std::tolower(sample[i]);
             }

            std::vector<std::string> suggested = trie->suggestCompletions(sample);
            if (capitalized) {
                for (int i=0; i < suggested.size(); i++) {
                    suggested[i][0] = std::toupper(suggested[i][0]);
                }
            }

            switch (suggested.size()) {
            case 1: {
                    suggestion1->setText(suggested[0].c_str());
                    suggestion2->setText("");
                    suggestion3->setText("");
                    break;
                }

            case 2: {
                    suggestion1->setText(suggested[0].c_str());
                    suggestion2->setText(suggested[1].c_str());
                    suggestion3->setText("");
                    break;
                }
            case 3: {
                    suggestion1->setText(suggested[0].c_str());
                    suggestion2->setText(suggested[1].c_str());
                    suggestion3->setText(suggested[2].c_str());
                    break;
                }
            }
        }

    }
    else {
        suggestion1->setText("");
        suggestion2->setText("");
        suggestion3->setText("");
    }
    
}
