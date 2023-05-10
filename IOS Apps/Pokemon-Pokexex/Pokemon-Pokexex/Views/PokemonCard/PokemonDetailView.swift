//
//  PokemonDetailView.swift
//  Pokedex
//
//  Created by David Rohweder on 10/24/22.
//

import SwiftUI

struct PokemonDetailView: View {
    @EnvironmentObject var manager : PokemonManager
    @Binding var pokemon : Pokemon

    var body: some View {
        ScrollView(.vertical) {
            VStack {
                Spacer()
                PokemonFeatureView(pokemon: pokemon)
                Spacer()
                PokemonEvolutionView(pokemon: pokemon)
                Spacer()
                PokemonListTypes(sectTitle: "Types", types: pokemon.types)
                PokemonListTypes(sectTitle: "Weaknesses", types: pokemon.weaknesses)
                Spacer()
            }
            .navigationTitle(Text(pokemon.name))
            .navigationBarTitleDisplayMode(.automatic)
        }
    }
}

struct PokemonDetailView_Previews: PreviewProvider {
    static var previews: some View {
        PokemonDetailView(pokemon: .constant(Pokemon.standard))
    }
}
