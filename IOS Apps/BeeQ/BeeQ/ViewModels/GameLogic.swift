//
//  GameLogic.swift
//  BeeQ
//
//  Created by user224354 on 9/6/22.
//

import Foundation

// Functions
extension BeeQEngine {
    
    func actionCommand (command: Int) -> Void {
        switch command {
        case 0:
            deleteCharacter()
        case 1:
            restartGame()
        case 2:
            submitPhase()
        default:
            restartGame()
        }
    }
    
    func charPressed (char: Character) -> Void {
        
        if currentGame.gameState == .inital {
            gameMechanics.buttons[0].disabled = false
            currentGame.gameState = .inGame
        }
        currentGame.currentWord += String(char)
        
        if isValidWord {
            gameMechanics.buttons[2].disabled = false
        } else {
            gameMechanics.buttons[2].disabled = true
        }
    }
    
    private func submitPhase () -> Void {
        currentSession.sessionScore += validWordScore
        
        if isPangram {
            currentSession.sessionScore += 5
        }
        
        if currentSession.sessionState == .inital {
            currentSession.sessionState = .inSession
        }
        
        // update session
        currentSession.foundWords.append(currentGame.currentWord)
        
        // restart game
        reconfigGame()
    }

    private func reconfigGame() {
        currentGame.currentWord = ""
        currentGame.gameState = .inital
        gameMechanics.buttons[0].disabled = true
        gameMechanics.buttons[1].disabled = false
        gameMechanics.buttons[2].disabled = true
    }
    
    private func deleteCharacter () -> Void {
        
        if !(currentGame.currentWord.count == 0)  {
            if currentGame.currentWord.count == 1 {
                gameMechanics.buttons[0].disabled = true
                currentGame.gameState = .inital
                currentGame.currentWord = ""
            }
            if !(currentGame.currentWord == "") {
                currentGame.currentWord.removeLast()
            }
            if !isValidWord {
                gameMechanics.buttons[2].disabled = true
            } else {
                gameMechanics.buttons[2].disabled = false
            }
        }
    }
    
    func restartGame () -> Void {
        currentGame = OneGame()
        currentSession = SessionModel()
        gameMechanics = GameMechanics()
    }
    
}
