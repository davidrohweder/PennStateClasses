//
//  CapturedPokemonView.swift
//  Pokedex
//
//  Created by David Rohweder on 10/28/22.
//

import SwiftUI

struct CapturedPokemonView: View {
    @EnvironmentObject var manager: PokemonManager
    @Environment(\.dismiss) private var dismiss
    var pokemon: Pokemon
    var body: some View {
        Button(action: {
            manager.capturedPokemons[pokemon.id]?.toggle()
            manager.pokemon[pokemon.id - 1].toggleCaptured()
            if manager.capturedPokemons[pokemon.id]! == false {
                dismiss()
            }
        }) {
            VStack {
                Text(pokemon.captured ? "Captured" : "Not Captured")
                    .tint(pokemon.captured ? .green : .red)
                Image(systemName: pokemon.captured ? "checkmark.circle" : "x.square")
                    .foregroundColor(pokemon.captured ? .green : .red)
            }
        }
    }
}

struct CapturedPokemonView_Previews: PreviewProvider {
    static var previews: some View {
        CapturedPokemonView(pokemon: Pokemon.standard)
    }
}
