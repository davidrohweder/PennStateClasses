//
//  CapturedListView.swift
//  Pokedex
//
//  Created by David Rohweder on 10/28/22.
//

import SwiftUI

struct CapturedListView: View {
    @EnvironmentObject var manager: PokemonManager
    var body: some View {
        if manager.capturedPokemon.count > 0 {
            Section {
                ScrollView (.horizontal) {
                    HStack {
                        ForEach(manager.capturedPokemon) { pokemon in
                            NavigationLink(destination: { PokemonDetailView(pokemon: $manager.pokemon[pokemon.id - 1])}){
                                CardPreviewView(pokemon: $manager.pokemon[pokemon.id - 1])
                                    .padding(5)
                            }
                        }
                    }
                }
            }
            header: {
                Text("Captured Pokemon")
                    .font(.title)
            }
        }
    }
}

struct CapturedListView_Previews: PreviewProvider {
    static var previews: some View {
        CapturedListView()
    }
}
