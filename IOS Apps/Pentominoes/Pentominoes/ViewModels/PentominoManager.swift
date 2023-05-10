//
//  PentominoManager.swift
//  Pentominoes
//
//  Created by user224354 on 9/14/22.
//

import Foundation
import UIKit

class PentominoManager: ObservableObject {

    @Published var boards: [Board]
    @Published var currentBoard: Int

    
    var solutions: Array<Dictionary<String, Position>>
    let numBoards: Int = 6
    @Published var pieces: [Piece]
    
    // JSON Read
    private let pieceStorageManager: StorageManager<[Tile]>
    private let solutionsStorageManager: StorageManager<[Dictionary<String, Position>]>
    
    init() {
        // read json tiles
        pieceStorageManager = StorageManager(name: "Tiles")
        let tiles = pieceStorageManager.modelData ?? []
        pieces = PentominoManager.generatePieces(tiles: tiles)
        
        currentBoard = 0
        boards = PentominoManager.generateBoards(boardCount: numBoards)
        
        // read json solutions
        solutionsStorageManager = StorageManager(name: "Solutions")
        solutions = solutionsStorageManager.modelData ?? []
    }
    
    static func generateBoards (boardCount: Int) -> [Board] {
        var boards: [Board] = []
        for i in 0..<boardCount {
            boards.append(Board(id: i))
        }
        return boards
    }
    
    // convert tile json input into pieces ...
    static func generatePieces(tiles: [Tile]) -> [Piece] {
        var id: Int = 0
        let minX: Int = Int((UIScreen.main.bounds.width / 2) - (Board.boardSize / 2))
        let minY: Int = Int(UIScreen.main.bounds.height / 2)
        var x: Int = 0
        var y: Int = 0
        var newPieces: [Piece] = []
        for tile in tiles {
            x = getPieceX(minX: minX, id: id)
            y = getPieceY(minY: minY, id: id)
            newPieces.append(Piece(uid: id, X: x, Y: y, initTile: tile))
            id += 1
        }
        return newPieces
    }
    
}
