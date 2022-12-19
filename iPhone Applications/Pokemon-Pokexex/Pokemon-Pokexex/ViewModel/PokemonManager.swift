//
//  PokemonManager.swift
//  Pokedex
//
//  Created by David Rohweder on 10/23/22.
//

import Foundation

class PokemonManager : ObservableObject {
    @Published var pokemon : [Pokemon]
    @Published var capturedPokemons: [Int: Bool]
    
    @Published var showSheet: Bool = false 
    
    let pokemon_storageManger : StorageManager<[Pokemon]>
    let capturedPokemon_storageManger : StorageManager<[Int: Bool]>
    
    init() {
        var filename = "pokedex"
        pokemon_storageManger = StorageManager<[Pokemon]>(name:filename)
        let _pokemon: [Pokemon]? = pokemon_storageManger.modelData ?? []
        filename = "captured"
        capturedPokemon_storageManger = StorageManager<[Int: Bool]>(name:filename)
        let _capturedPokemon: [Int: Bool]? = capturedPokemon_storageManger.modelData ?? [:]
        pokemon = []
        capturedPokemons = [:]
        
        guard _pokemon != nil else {return}
        pokemon = _pokemon!.sorted { $0.id < $1.id }
        
        guard _capturedPokemon != nil else {return}
        capturedPokemons = _capturedPokemon!
        
        pokemon = generatePokemon(_pokemon: pokemon,_capturedPokemon: capturedPokemons)
    }
    
    func generatePokemon(_pokemon: [Pokemon], _capturedPokemon: [Int: Bool]) -> [Pokemon] {
        var localPokemon: [Pokemon] = []
        
        for p in _pokemon {
            localPokemon.append(p)
            if capturedPokemons[p.id] != nil {
                if capturedPokemons[p.id]! {
                    localPokemon[p.id - 1].captured = true
                }
            } else {
                capturedPokemons[p.id] = false
            }
        }
        return localPokemon
    }
    
}
