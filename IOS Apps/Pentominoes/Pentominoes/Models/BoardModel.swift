//
//  BoardModel.swift
//  Pentominoes
//
//  Created by user224354 on 9/15/22.
//

import Foundation
import SwiftUI

struct Board: Identifiable {
    var id: Int { buttonID }
    var buttonID: Int
    var boardName: String
    var boardButtonName: String
    
    static let boardSize: CGFloat = 560
    
    init (id: Int) {
        buttonID = id
        boardName = "Board\(id)"
        boardButtonName = "Board\(id)button"
    }
    
    var buttonLocationX: CGFloat {
        let centerX = UIScreen.main.bounds.width / 2
        return buttonID % 2 == 0 ? centerX - (Board.boardSize / 2) - 115 : centerX + (Board.boardSize / 2) + 65
    }
    
    var buttonLocationY: CGFloat {
        let centerY = UIScreen.main.bounds.height / 2 - 250
        return  centerY - (Board.boardSize / 2) + CGFloat(Int(buttonID / 2) * 125)
    }
    
}
