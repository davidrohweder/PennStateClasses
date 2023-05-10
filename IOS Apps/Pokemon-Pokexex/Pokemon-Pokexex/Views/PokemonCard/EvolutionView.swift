//
//  EvolutionView.swift
//  Pokedex
//
//  Created by David Rohweder on 10/31/22.
//

import SwiftUI

struct EvolutionView: View {
    @EnvironmentObject var manager: PokemonManager
    var ids: [Int]
    var body: some View {
        ForEach(ids, id: \.self) { id in
            NavigationLink(destination: { PokemonDetailView(pokemon: $manager.pokemon[id - 1])}){
                CardPreviewView(pokemon: $manager.pokemon[id - 1])
                    .padding()
            }
        }
        
    }
}

struct EvolutionView_Previews: PreviewProvider {
    static var previews: some View {
        EvolutionView(ids: [0])
    }
}
