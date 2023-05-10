//
//  Gradient+Colors.swift
//  Pokedex
//
//  Created by David Rohweder on 10/25/22.
//

import Foundation
import SwiftUI

extension LinearGradient {
    init(types: [PokemonType]) {
        
        var _colors: [Color] = []
        
        for type in types {
            _colors.append(Color(pokemonType: type))
            
        }
                
        self = LinearGradient(colors: _colors, startPoint: .top, endPoint: .bottom)
        
    }
}
