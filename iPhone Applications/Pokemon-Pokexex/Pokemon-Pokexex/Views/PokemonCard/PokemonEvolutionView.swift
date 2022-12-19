//
//  PokemonEvolutionView.swift
//  Pokedex
//
//  Created by David Rohweder on 10/27/22.
//

import SwiftUI

struct PokemonEvolutionView: View {
    var pokemon: Pokemon
    var body: some View {
        VStack {
            Spacer()
            if pokemon.prev_evolution != nil || pokemon.next_evolution != nil
            {
                Text("Evolutions").italic()
            } else {
                Text("No Evolutions").italic()
            }
            HStack {
                if pokemon.prev_evolution != nil {
                    EvolutionView(ids: pokemon.prev_evolution!)
                }
                
                if pokemon.next_evolution != nil {
                    EvolutionView(ids: pokemon.next_evolution!)
                }
            }
        }
    }
}

struct PokemonEvolutionView_Previews: PreviewProvider {
    static var previews: some View {
        PokemonEvolutionView(pokemon: Pokemon.standard)
    }
}
