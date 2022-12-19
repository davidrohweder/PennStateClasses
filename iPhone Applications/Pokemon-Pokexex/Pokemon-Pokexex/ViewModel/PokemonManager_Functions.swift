//
//  SwiftUIView.swift
//  Pokedex
//
//  Created by David Rohweder on 10/27/22.
//

import Foundation

extension PokemonManager {
    
    func pokemonOfType(type: PokemonType) -> [Pokemon] {
        return pokemon.filter { $0.types.contains(type) }
    }
    
}
