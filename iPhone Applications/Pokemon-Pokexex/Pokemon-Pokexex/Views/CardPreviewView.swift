//
//  CardPreviewView.swift
//  Pokedex
//
//  Created by David Rohweder on 10/27/22.
//

import SwiftUI

struct CardPreviewView: View {
    @EnvironmentObject var manager: PokemonManager
    @Binding var pokemon : Pokemon
    let size: CGFloat = 75
    var body: some View {
        VStack {
            PokemonImageView(imageName: pokemon.imageName, size: size, types: pokemon.types)
                .frame(width: size, height: size)
                .tint(LinearGradient(types: pokemon.types))
                .fontWeight(.light)
            Text(pokemon.name)
                .fontWeight(.heavy)
                .tint(LinearGradient(types: pokemon.types))
            Text(pokemon.captured ? "Captured" : "Not Captured")
                .tint(pokemon.captured ? .green : .red)
                .font(.system(size: 10))
                .italic()
        }
    }
}

struct CardPreviewView_Previews: PreviewProvider {
    static var previews: some View {
        CardPreviewView(pokemon: .constant(Pokemon.standard))
    }
}
