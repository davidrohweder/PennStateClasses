//
//  PokemonListTypes.swift
//  Pokedex
//
//  Created by David Rohweder on 10/25/22.
//

import SwiftUI

struct PokemonListTypes: View {
    var sectTitle: String
    var types: [PokemonType]
    var body: some View {
        VStack (alignment: .leading) {
            Text(sectTitle)
                .bold()
                .fontWeight(.medium)
            ScrollView (.horizontal) {
                HStack {
                    ForEach (types) { type in
                        SingleTypeCapsuleView(type: type)
                    }
                }
            }
        }
        
    }
}

struct PokemonListTypes_Previews: PreviewProvider {
    static var previews: some View {
        PokemonListTypes(sectTitle: "", types: [.bug])
    }
}
