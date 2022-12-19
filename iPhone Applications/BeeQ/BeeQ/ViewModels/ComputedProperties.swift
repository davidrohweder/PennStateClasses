//
//  ComputedProperties.swift
//  BeeQ
//
//  Created by user224354 on 9/6/22.
//

import Foundation

// Computer Properties
extension BeeQEngine {
    
    var isValidWord: Bool {
         var valid: Bool = false
        
        let db: [String] = PreferencesModel.language == .english ? Words.words : frenchWords
         for word in db {
             if currentGame.currentWord == word && !currentSession.foundWords.contains(currentGame.currentWord) && currentGame.currentWord.contains(currentGame.mustContains) {
                 valid = true
                 break
             }
         }
         for found in currentSession.foundWords {
             if found == currentGame.currentWord {
                 valid = false
                 break
             }
         }
         return valid
     }
    
    var validWordScore: Int { currentGame.currentWord.count == 4 ? 1 : currentGame.currentWord.count }
    
    var isPangram: Bool {
        if currentGame.hasPangram(word: currentGame.currentWord, db: currentGame.availableCharacters) {
            return true
        } else {
            return false
        }
    } 
        
}
