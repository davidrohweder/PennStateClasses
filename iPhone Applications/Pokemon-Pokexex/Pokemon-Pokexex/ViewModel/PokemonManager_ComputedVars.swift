//
//  PokemonManager_ComputedVars.swift
//  Pokedex
//
//  Created by David Rohweder on 10/28/22.
//

import Foundation

extension PokemonManager {
    
    var capturedPokemon: [Pokemon] {
        pokemon.filter { $0.captured }
    }
    
}
