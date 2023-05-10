//
//  PentominoManager_Functions.swift
//  Pentominoes
//
//  Created by user224354 on 9/14/22.
//

import Foundation
import SwiftUI

// Pentomino Manager Functions
extension PentominoManager {
    
    func MechanicFunctions(id: Int) -> Void {
        switch id {
        case 6: resetGame()
        case 7: solveGame()
        default: boardSwitch(id: id)
        }
        
    }
    
    func boardSwitch(id: Int) {
        currentBoard = id
        resetGame()
    }
    
    func resetGame() {
        let minX: Int = Int((UIScreen.main.bounds.width / 2) - (Board.boardSize / 2))
        let minY: Int = Int(UIScreen.main.bounds.height / 2)
        for id in 0..<(pieces.count) {
            pieces[id].position.x = PentominoManager.getPieceX(minX: minX, id: id)
            pieces[id].position.y = PentominoManager.getPieceY(minY: minY, id: id)
            pieces[id].position.rotations = 0
            pieces[id].position.isFlipped = false
        }
    }
    
    func solveGame() {
        let boardSolution = solutions[currentBoard - 1] // board zero is not in sol set
        for pieceSolution in boardSolution {
            let cPiece = pieces.first(where: { $0.tile.name == pieceSolution.key } )
            let pId: Int = cPiece!.bid
            pieces[pId].position.x = pieceSolution.value.x + 5
            pieces[pId].position.y = pieceSolution.value.y + 1
            pieces[pId].position.rotations = pieceSolution.value.rotations
            pieces[pId].position.isFlipped = pieceSolution.value.isFlipped
        }
    } // change all positions to the correct
    
    class func getPieceX(minX: Int, id: Int) -> Int {
        return (minX + 40 + Int(Board.boardSize / 4) * (id % 4)) / 40
    }
    
    class func getPieceY(minY: Int, id: Int) -> Int {
        return (minY + (id / 4  * 200)) / 40
    }
    
}
