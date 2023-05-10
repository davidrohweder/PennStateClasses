//
//  PokemonFeatureView.swift
//  Pokedex
//
//  Created by David Rohweder on 10/25/22.
//

import SwiftUI

struct PokemonFeatureView: View {
    var pokemon: Pokemon
    let size: CGFloat = 350
    var body: some View {
        VStack {
            PokemonImageView(imageName: pokemon.imageName, size: size , types: pokemon.types)
                .overlay(Text(pokemon.imageName).offset(x: -20, y: -4), alignment: .bottomTrailing)
                .frame(width: size, height: size, alignment: .topLeading)
            HStack {
                Spacer()
                PokemonInfoSect(sectName: "Height", value: pokemon.height, unitDisplay: "m")
                Spacer()
                PokemonInfoSect(sectName: "Weight", value: pokemon.weight, unitDisplay: "kg")
                Spacer()
                CapturedPokemonView(pokemon: pokemon)
                Spacer()
            }
        }
    }
}

struct PokemonInfoSect: View {
    var sectName: String
    var value: Double
    var unitDisplay: String
    var body: some View {
        VStack {
            Text(sectName)
            HStack {
                Text("\(value, specifier: "%.2f")")
                    .bold()
                    .font(.title2)
                Text(unitDisplay)
            }
        }
    }
}

struct PokemonFeatureView_Previews: PreviewProvider {
    static var previews: some View {
        PokemonFeatureView(pokemon: Pokemon.standard)
    }
}
