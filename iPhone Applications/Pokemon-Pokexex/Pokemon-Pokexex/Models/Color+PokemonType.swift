//
//  Color+PokemonType.swift
//  Pokedex
//
//  Created by Goki on 9/19/20.
//  Copyright © 2020 Goki. All rights reserved.
//

import SwiftUI

extension Color {
    init(pokemonType: PokemonType) {
        switch pokemonType {
        case .bug:
            self = Color(#colorLiteral(red: 0.568513453, green: 0.7608199716, blue: 0.1568415463, alpha: 1))
        case .dragon:
            self = Color(#colorLiteral(red: 0.01125660352, green: 0.4274889231, blue: 0.7724513412, alpha: 1))
        case .electric:
            self = Color(#colorLiteral(red: 0.9528874755, green: 0.8274896741, blue: 0.2196110189, alpha: 1))
        case .fighting:
            self = Color(#colorLiteral(red: 0.8117805123, green: 0.2431584001, blue: 0.4117267728, alpha: 1))
        case .fire:
            self = Color(#colorLiteral(red: 0.9999889731, green: 0.6157169342, blue: 0.3254770637, alpha: 1))
        case .flying:
            self = Color(#colorLiteral(red: 0.5646253228, green: 0.6667106152, blue: 0.8704809546, alpha: 1))
        case .ghost:
            self = Color(#colorLiteral(red: 0.3175879419, green: 0.4117984772, blue: 0.6822669506, alpha: 1))
        case .grass:
            self = Color(#colorLiteral(red: 0.3880509138, green: 0.7372908592, blue: 0.3568112254, alpha: 1))
        case .ground:
            self = Color(#colorLiteral(red: 0.8548993468, green: 0.4706124663, blue: 0.2666578889, alpha: 1))
        case .ice:
            self = Color(#colorLiteral(red: 0.4586400986, green: 0.8118094206, blue: 0.7567664981, alpha: 1))
        case .normal:
            self = Color(#colorLiteral(red: 0.5685663819, green: 0.6039572358, blue: 0.6352155805, alpha: 1))
        case .poison:
            self = Color(#colorLiteral(red: 0.6745018363, green: 0.4157250524, blue: 0.7881407738, alpha: 1))
        case .psychic:
            self = Color(#colorLiteral(red: 0.976477921, green: 0.4470857978, blue: 0.4666262865, alpha: 1))
        case .rock:
            self = Color(#colorLiteral(red: 0.7842597365, green: 0.7216063738, blue: 0.548955977, alpha: 1))
        case .water:
            self = Color(#colorLiteral(red: 0.297898829, green: 0.5686688423, blue: 0.8391104341, alpha: 1))
        }
    }
}
