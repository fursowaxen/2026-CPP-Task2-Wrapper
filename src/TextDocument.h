#pragma once

#include <cctype>
#include <string>
#include <utility>

class TextDocument {
private:
    static bool is_word_char(char ch) {
        const unsigned char c = static_cast<unsigned char>(ch);
        return std::isalnum(c) || ch == '_';
    }
    std::string m_text;

public:
    explicit TextDocument(std::string text = {}) : m_text(std::move(text)) {}

    const std::string& get_text() const { return m_text; }

    std::string replace_words(const std::string& old_word,
        const std::string& new_word) {
        if (old_word.empty()) return m_text;

        std::size_t pos = 0;
        while ((pos = m_text.find(old_word, pos)) != std::string::npos) {
            const bool left_boundary = pos == 0 || !is_word_char(m_text[pos - 1]);
            const std::size_t end = pos + old_word.size();
            const bool right_boundary = end == m_text.size() || !is_word_char(m_text[end]);
            if (left_boundary && right_boundary) {
                m_text.replace(pos, old_word.size(), new_word);
                pos += new_word.size();
            }
            else {
                pos += old_word.size();
            }
        }
        return m_text;
    }

    std::string find_phrase(const std::string& phrase) const {
        if (phrase.empty()) return "Phrase is empty";
        std::string result;
        std::size_t pos = m_text.find(phrase);
        while (pos != std::string::npos) {
            if (!result.empty()) result += ", ";
            result += std::to_string(pos);
            pos = m_text.find(phrase, pos + 1);
        }
        return result.empty() ? "Not found" : result;
    }

    std::string capitalize_words() {
        bool beginning_of_word = true;
        for (char& ch : m_text) {
            const unsigned char c = static_cast<unsigned char>(ch);
            if (is_word_char(ch)) {
                if (beginning_of_word) ch = static_cast<char>(std::toupper(c));
                beginning_of_word = false;
            }
            else {
                beginning_of_word = true;
            }
        }
        return m_text;
    }

    std::string capitalize_sentences() {
        bool need_capital = true;
        for (char& ch : m_text) {
            const unsigned char c = static_cast<unsigned char>(ch);
            if (need_capital && std::isalpha(c)) {
                ch = static_cast<char>(std::toupper(c));
                need_capital = false;
            }
            if (ch == '.' || ch == '!' || ch == '?') {
                need_capital = true;
            }
        }
        return m_text;
    }

    std::string number_sentences() {
        std::string result;
        std::size_t sentence_number = 1;
        bool at_sentence_start = true;

        for (std::size_t i = 0; i < m_text.size(); ++i) {
            const char ch = m_text[i];
            if (at_sentence_start && !std::isspace(static_cast<unsigned char>(ch))) {
                if (!result.empty()) result += '\n';
                result += std::to_string(sentence_number++) + ". ";
                at_sentence_start = false;
            }
            result += ch;
            if (ch == '.' || ch == '!' || ch == '?') {
                std::size_t j = i + 1;
                while (j < m_text.size() &&
                    (m_text[j] == '.' || m_text[j] == '!' || m_text[j] == '?')) {
                    result += m_text[j++];
                }
                i = j - 1;
                at_sentence_start = true;
            }
        }
        m_text = std::move(result);
        return m_text;
    }
};
