//
//  Pokemon_PokexexApp.swift
//  Pokemon-Pokexex
//
//  Created by David Rohweder on 11/1/22.
//

import SwiftUI

@main
struct Pokemon_PokexexApp: App {
    @StateObject var manager: PokemonManager = PokemonManager()
    @Environment(\.scenePhase) private var scenePhase
    var body: some Scene {
        WindowGroup {
            RootView()
                .environmentObject(manager)
        }
        .onChange(of: scenePhase, perform: { scene in
            if scene == .inactive {
                manager.capturedPokemon_storageManger.save(manager.capturedPokemons)
            }
        })
    }
}
