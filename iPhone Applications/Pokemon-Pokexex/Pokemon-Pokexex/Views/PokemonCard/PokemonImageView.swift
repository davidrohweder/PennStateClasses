//
//  PokemonImageView.swift
//  Pokedex
//
//  Created by David Rohweder on 10/25/22.
//

import SwiftUI

struct PokemonImageView: View {
    var imageName: String
    var size: Double
    var types: [PokemonType]
    var body: some View {
        ZStack {
            Rectangle()
                .fill(LinearGradient(types: types))
                .clipShape(RoundedRectangle(cornerRadius: 25, style: .continuous))
            Image(imageName)
                .resizable()
                .aspectRatio(contentMode: .fill)
                .frame(width: size - 25, height: size - 25)
        }
    }
}

struct PokemonImageView_Previews: PreviewProvider {
    static var previews: some View {
        PokemonImageView(imageName: "", size: 0.0 , types: [.bug])
    }
}
