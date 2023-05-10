//
//  Pentominoes.swift
//  Assets for Assignment 4
//
//  Created by Hannan, John Joseph on 9/7/22.
//

import Foundation

// a tile with width/height in unit coordinates
struct Tile: Codable {
    let name : String
    let width : Int
    let height : Int
    
    static let standard = Tile(name: "I", width: 1, height: 4)
}

// specifies the complete orientation of a piece using unit coordinates
struct Position: Codable  {
    var x : Int
    var y : Int
    var isFlipped : Bool
    var rotations : Int
    
    init(coordX: Int, coordY: Int) {
        x = coordX
        y = coordY
        isFlipped = false
        rotations = 0
    }
    
}

// a Piece is the model data that the view uses to display a pentomino
struct Piece : Identifiable {
    var id: Int { bid }
    var bid: Int
    var tile : Tile
    var position : Position
    let blockSize = 40
    var pieceState: pieceStates
    
    enum pieceStates {
        case inital, moved
    }
    
    var centerPosition : (Double, Double) {
        // calculate the cneter of the peice
        let width = position.rotations % 2 == 0 ? tile.width : tile.height
        let height = position.rotations % 2 == 0 ? tile.height : tile.width
        let x = (Double(position.x) + Double(width) / 2.0) * Double(blockSize)
        let y = (Double(position.y) + Double(height) / 2.0) * Double(blockSize)
        return (x,y)
    }
    
    init(uid: Int, X: Int, Y: Int, initTile: Tile) {
        bid = uid
        tile = initTile
        position = Position(coordX: X, coordY: Y)
        pieceState = .inital
    }
    

}
