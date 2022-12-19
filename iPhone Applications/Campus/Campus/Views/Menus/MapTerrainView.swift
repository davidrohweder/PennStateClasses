//
//  AllFavoritesView.swift
//  Campus
//
//  Created by user224354 on 10/3/22.
//

import SwiftUI

struct MapTerrainView: View {
    @EnvironmentObject var manager: MapManager
    var body: some View {
        
        Menu {
            ForEach(TerrainTypes.allCases) { type in
                Button (action: {
                    manager.setTerrtainType(type: type)
                    print(type)
                }) {
                    HStack {
                        Text(type.rawValue)
                        Image(systemName: type == .standard ? "globe.americas" : (type == .hybrid ? "globe.americas.fill" : "globe.asia.australia.fill"))
                    }
                }
            }
        } label: {
            Image(systemName: manager.terrainType == .standard ? "globe.americas" : (manager.terrainType == .hybrid ? "globe.americas.fill" : "globe.asia.australia.fill"))
        }
    }
}

struct MapTerrainViewView_Previews: PreviewProvider {
    static var previews: some View {
        MapTerrainView()
    }
}
