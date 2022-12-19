//
//  BeeQEngine.swift
//  BeeQ
//
//  Created by user224354 on 8/25/22.
//

import Foundation

class BeeQEngine: ObservableObject {
    
    // Declare Models
    @Published var currentSession: SessionModel = SessionModel()
    @Published var gameMechanics: GameMechanics = GameMechanics()
    @Published var currentGame: OneGame = OneGame()
    @Published var sessionPreferences: PreferencesModel = PreferencesModel()
}
