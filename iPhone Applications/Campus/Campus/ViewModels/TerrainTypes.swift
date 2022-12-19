//
//  TerrainTypes.swift
//  Campus
//
//  Created by user224354 on 10/3/22.
//

import Foundation

enum TerrainTypes: String, Identifiable, CaseIterable {
    case standard = "Standard Map", hybrid = "Hybrid Map", imagery = "Satellite Map"
    var id: RawValue {rawValue}
}
