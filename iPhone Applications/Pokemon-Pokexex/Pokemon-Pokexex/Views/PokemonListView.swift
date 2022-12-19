//
//  PokemonListView.swift
//  Pokedex
//
//  Created by David Rohweder on 10/23/22.
//

import SwiftUI

struct PokemonListView: View {
    @EnvironmentObject var manager : PokemonManager
    var queryMech: [PokemonType]
    var body: some View {
        List {
            CapturedListView()
            ForEach (queryMech) { type in
                Section {
                    ScrollView (.horizontal) {
                        HStack {
                            ForEach(manager.pokemonOfType(type: type)) { pokemon in
                                NavigationLink(destination: {PokemonDetailView(pokemon: $manager.pokemon[pokemon.id - 1])}){
                                    CardPreviewView(pokemon: $manager.pokemon[pokemon.id - 1])
                                }
                            }
                        }
                    }
                } header: {
                    Text(type.rawValue)
                        .font(.title)
                        .foregroundColor(Color(pokemonType: type))
                }
            }
        }
        .navigationTitle("Pokedex")
        .navigationBarTitleDisplayMode(.inline)
        .sheet(isPresented: $manager.showSheet) {
            FilterPickerView()
        }
        .toolbar {
            ToolbarItem() {
                Button(action: {
                    manager.showSheet.toggle()
                }) {
                    Image(systemName: "gear")
                }
            }
        }
    }
}

struct StateListView_Previews: PreviewProvider {
    static var previews: some View {
        PokemonListView(queryMech: [.bug])
    }
}
