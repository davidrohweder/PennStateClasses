//
//  SectionTypes.swift
//  Pokedex
//
//  Created by David Rohweder on 10/31/22.
//

import Foundation


enum SectionTypes: String, CaseIterable, Identifiable {
    var id: String { self.rawValue }
    case none = "None"
    case specific = "Specific"
}
