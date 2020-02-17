#include <iostream>
#include <chrono>
#include "nlp.h"

int main()
{
    // const std::wstring sample{L"Hello, Nikte! How are you doing today? I'm so tired of working. That's right... Let's go out somedayHello, Nikte! How are you doing today? I'm so tired of working. That's right...Hello, Nikte! How are you doing today? I'm so tired of working. That's right...Hello, Nikte! How are you doing today? I'm so tired of working. That's right...Hello, Nikte! How are you doing today? I'm so tired of working. That's right...Hello, Nikte! How are you doing today? I'm so tired of working. That's right...Hello, Nikte! How are you doing today? I'm so tired of working. That's right...Hello, Nikte! How are you doing today? I'm so tired of working. That's right...Hello, Nikte! How are you doing today? I'm so tired of working. That's right..."};
    // const std::wstring sample{L"The world is changing, nothing will ever be the same in our village. The world is changing, nothing will ever be the same in our village. The world is changing, nothing will ever be the same in our village. The world is changing, nothing will ever be the same in our village. The world is changing, nothing will ever be the same in our village. The world is changing, nothing will ever be the same in our village. The world is changing, nothing will ever be the same in our village. The world is changing, nothing will ever be the same in our village. The world is changing, nothing will ever be the same in our village. The world is changing, nothing will ever be the same in our village. The world is changing, nothing will ever be the same in our village. The world is changing, nothing will ever be the same in our village. The world is changing, nothing will ever be the same in our village."};
    const std::wstring sample{L"Hello, Nikte! How are you doing today? I'm so tired of working. That's right... Let's go out someday"};
    stella::nlp::NLP nlp{"./perceptron-tagger"};
    
    const int NUMBER_OF_TESTS = 40;
    for (int i = 0; i < NUMBER_OF_TESTS; ++i)
    {
        auto t1 = std::chrono::high_resolution_clock::now();
        auto tokens = nlp.analyze(sample);
        auto t2 = std::chrono::high_resolution_clock::now();

        auto duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();
        std::wcout << "Execution of NLP analysis took " << duration/1000000.f << " seconds.\n";
    }
    
    // auto tokens = nlp.analyze(sample);

    // for (const auto& token : tokens)
    // {
    //     if (!token.text.empty())
    //     {
    //         std::wcout << token.text << L' ' << token.pos_tag << L' ' << token.lemma << L'\n';
    //     }
    // }
    
    return 0;
}