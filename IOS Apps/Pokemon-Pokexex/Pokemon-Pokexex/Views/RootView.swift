//
//  RootView.swift
//  Pokedex
//
//  Created by David Rohweder on 10/22/22.
//

import SwiftUI

struct RootView: View {
    @EnvironmentObject var manager : PokemonManager
    @AppStorage("Constants.Preferences.sectioning") var sectioning : SectionTypes = .none
    @AppStorage("Constants.Preferences.filterBy") var filterBy : PokemonType = .bug

    var body: some View {
        NavigationStack {
            if sectioning == .none {
               PokemonListView(queryMech: PokemonType.allCases)
            } else {
                PokemonListView(queryMech: [filterBy])
            }
        }
    }
}

struct RootView_Previews: PreviewProvider {
    static var previews: some View {
        RootView()
    }
}
