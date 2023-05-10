//
//  SessionModel.swift
//  BeeQ
//
//  Created by user224354 on 9/6/22.
//

import Foundation

struct SessionModel {
    var foundWords: [String]
    var sessionScore: Int
    var sessionState: SessionStates
    
    enum SessionStates {
        case inital, inSession
    }
    
    init () {
        foundWords = []
        sessionScore = 0
        sessionState = .inital
    }
}
