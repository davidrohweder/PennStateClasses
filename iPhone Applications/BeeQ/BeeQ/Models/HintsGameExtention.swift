//
//  HintsGameExtention.swift
//  BeeQ
//
//  Created by user224354 on 9/8/22.
//

import Foundation


extension OneGame {
    
    var possibleWords: [String] {
        let db = PreferencesModel.language == .english ? Words.words : frenchWords
        var validChars: Bool = false
        var words: [String] = []
        for word in db {
            validChars = true
            for char in word {
                if !availableCharacters.contains(char) {
                    validChars = false
                }
            }
            if validChars {
                words.append(word)
            }
        }
        return words
    }
    
    var totalPossibleWords: Int {
        possibleWords.count
    }
    
    var possiblePangrams: Int {
        let db = possibleWords
        var pangramTotal: Int = 0
        for word in db {
            if hasPangram(word: word, db: db) {
                pangramTotal += 1
            }
        }
        return pangramTotal
    }
    
    var totalPossiblePoints: Int {
        let db = possibleWords
        var totalPoints: Int = 0
        for word in db {
            if hasPangram(word: word, db: db) {
                totalPoints += word.count + pangramBonus
            } else {
                totalPoints += word.count
            }
        }
        return totalPoints
    }
    
    func totalPossibleWordFromChar (char: Character, length: Int) -> Int {
        let db = PreferencesModel.language == .english ? Words.words : frenchWords
        var totalMatching: Int = 0
        for word in db {
            if word.prefix(1) == String(char) && word.count == length {
                totalMatching += 1
            }
        }
        return totalMatching
    }

}
