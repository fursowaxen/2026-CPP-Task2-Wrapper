#include "Engine.h"
#include "TextDocument.h"
#include <iostream>
#include <string>

class Subject {
public:
    std::string func(int number, const std::string& text) {
        return "Number: " + std::to_string(number) + ", Text: " + text;
    }
    int sum(int first, int second) const {
        return first + second;
    }
};

int main() {
    try {
        std::cout << "Example 1. Basic usage\n";
        Subject subj;
        Engine engine;

        Wrapper wrapper(&subj, &Subject::func, { {"number", 0}, {"text", "a"} });
        engine.register_command(&wrapper, "func");

        std::cout << "Full override:  " << engine.execute("func", { {"text", "b"}, {"number", 4} }) << '\n';
        std::cout << "Partial:        " << engine.execute("func", { {"number", 5} }) << '\n';
        std::cout << "Defaults:       " << engine.execute("func") << '\n';

        engine.register_command(std::make_unique<Wrapper>(&subj, &Subject::sum, Arguments{ {"first", 0}, {"second", 0} }), "sum");
        std::cout << "Other method:   " << engine.execute("sum", { {"second", 5}, {"first", 4} }) << "\n";


        std::cout << "Example 2. TextDocument \n";
        TextDocument doc("This is a test text. Hello! How is your day?");
        std::cout << "Original: \"" << doc.get_text() << "\"\n";

        Wrapper replace_w(&doc, &TextDocument::replace_words, { {"old_word", "hello"}, {"new_word", "hi"} });
        Wrapper find_w(&doc, &TextDocument::find_phrase, { {"phrase", "test"} });
        Wrapper cap_words_w(&doc, &TextDocument::capitalize_words, {});
        Wrapper cap_sents_w(&doc, &TextDocument::capitalize_sentences, {});
        Wrapper number_w(&doc, &TextDocument::number_sentences, {});

        engine.register_command(&replace_w, "replace");
        engine.register_command(&find_w, "find_phrase");
        engine.register_command(&cap_words_w, "capitalize_words");
        engine.register_command(&cap_sents_w, "capitalize_sentences"); 
        engine.register_command(&number_w, "number");

        std::cout << "Replace:        " << engine.execute("replace", { {"new_word", "Let's go!!!"}, {"old_word", "Hello!"} }) << '\n';
        std::cout << "Find positions: " << engine.execute("find_phrase") << '\n';
        std::cout << "Capitalize words:      " << engine.execute("capitalize_words") << '\n';
        std::cout << "Capitalize sentences:  " << engine.execute("capitalize_sentences") << '\n';
        std::cout << "Numbering:\n" << engine.execute("number") << '\n';

    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << '\n';
        return 1;
    }
    return 0;
}