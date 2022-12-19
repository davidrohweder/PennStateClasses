//
//  PokemonType.swift
//  Pokedex
//
//  Created by Goki on 9/19/20.
//  Copyright © 2020 Goki. All rights reserved.
//

enum PokemonType: String, Codable, Identifiable, CaseIterable {
    var id: String { self.rawValue }
    
    case bug = "Bug"
    case dragon = "Dragon"
    case electric = "Electric"
    case fighting = "Fighting"
    case fire = "Fire"
    case flying = "Flying"
    case ghost = "Ghost"
    case grass = "Grass"
    case ground = "Ground"
    case ice = "Ice"
    case normal = "Normal"
    case poison = "Poison"
    case psychic = "Psychic"
    case rock = "Rock"
    case water = "Water"
}
