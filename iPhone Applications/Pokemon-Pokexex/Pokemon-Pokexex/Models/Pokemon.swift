//
//  Pokemon.swift
//  Pokedex
//
//  Created by David Rohweder on 10/24/22.
//

import Foundation

struct Pokemon :Identifiable, Encodable {
    let id: Int
    let name : String
    let height: Double
    let weight : Double
    let types: [PokemonType]
    let weaknesses: [PokemonType]
    let prev_evolution: [Int]?
    let next_evolution: [Int]?
    var captured: Bool
    
    var imageName: String {
        return String(format: "%03d", id)
    }
    
    mutating func toggleCaptured() {
        captured.toggle()
    }
    
    static let standard = Pokemon(id: 0, name: "", height: 0.0, weight: 0.0, types: [.bug], weaknesses: [.bug], prev_evolution: [], next_evolution: [], captured: false)
    
    enum CodingKeys : String, CodingKey {
        case id = "id", name = "name", height = "height", weight = "weight"
        case types = "types", weaknesses = "weaknesses"
        case prev_evolution = "prev_evolution", next_evolution = "next_evolution"
    }
    
}

extension Pokemon : Decodable {
    init(from decoder: Decoder) throws {
        let values = try decoder.container(keyedBy: CodingKeys.self)
        id = try values.decode(Int.self, forKey: .id)
        name = try values.decode(String.self, forKey: .name)
        height = try values.decode(Double.self, forKey: .height)
        weight = try values.decode(Double.self, forKey: .weight)
        types = try values.decode([PokemonType].self, forKey: .types)
        weaknesses = try values.decode([PokemonType].self, forKey: .weaknesses)
        prev_evolution = try values.decodeIfPresent([Int].self, forKey: .prev_evolution)
        next_evolution = try values.decodeIfPresent([Int].self, forKey: .next_evolution)
        captured = false
    }
}
