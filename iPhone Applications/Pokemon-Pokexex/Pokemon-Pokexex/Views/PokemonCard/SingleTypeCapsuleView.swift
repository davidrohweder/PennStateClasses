//
//  SingleTypeCapsuleView.swift
//  Pokedex
//
//  Created by David Rohweder on 10/25/22.
//

import SwiftUI

struct SingleTypeCapsuleView: View {
    var type: PokemonType
    var body: some View {
        HStack {
            Spacer()
            Text(type.rawValue)
                .frame(width: 75, height: 50)
                .background(Capsule().fill(Color(pokemonType: type)))
            Spacer()
        }
    }
}

struct SingleTypeCapsuleView_Previews: PreviewProvider {
    static var previews: some View {
        SingleTypeCapsuleView(type: .bug)
    }
}
