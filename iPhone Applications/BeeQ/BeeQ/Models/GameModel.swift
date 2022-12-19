//
//  GameModel.swift
//  BeeQ
//
//  Created by user224354 on 8/31/22.
//

import Foundation

struct OneGame {
    var currentWord: String
    var availableCharacters: [Character]
    var gameState: gameStates
    let pangramBonus: Int = 5
    static var characterOptions: Int = 5
    let characterPossible: [Int] = [4,5,6,7]
    var mustContains: Character
    
    enum gameStates {
        case inital, inGame
    }
    
    init() {
        gameState = .inital
        currentWord = ""
        availableCharacters = OneGame.generateDistinctCharacters()
        mustContains = availableCharacters[0]
    }
    
    static func generateDistinctCharacters () -> [Character] {

        let randomWord: String = (Words.words.filter{Set($0).count == OneGame.characterOptions}).randomElement()!
        let chars = Array(Set(randomWord)).shuffled()
        return chars
        
    }
      
    func hasPangram<T>(word: String, db: T) -> Bool {
        var eq: Int = 0 // equivilant chars to available chars if equal to character options then pangram is true
        var lettersContained: [String] = [] // make sure that no duplicate characters are counted for eq i.e. the equivilant chars
        for char in word {
            if availableCharacters.contains(char) && !(lettersContained.contains(String(char))) {
                eq += 1
                lettersContained.append(String(char))
            }
        }
        if eq == OneGame.characterOptions {
            return true
        } else {
            return false
        }
    }
    
}
