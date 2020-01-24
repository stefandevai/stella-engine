#!/usr/bin/env python
# -*- coding: utf-8 -*-

import npc

def chat():
    sentence = input("Say something to Itzé: ")
    answer = "a"
    if sentence:
        print("YOU: " + sentence)
        while sentence and sentence != "bye" and answer != "":
            answer = npc.itze.answer(sentence)
            if answer:
                print(npc.itze.name.upper() + ": " + answer + '\n')
                sentence = input("YOU: ")
        if answer and npc.itze.greeted:
            print(npc.itze.name.upper() + ": " + npc.itze.bye())

def main():
    chat()

if __name__ == "__main__":
    main()
