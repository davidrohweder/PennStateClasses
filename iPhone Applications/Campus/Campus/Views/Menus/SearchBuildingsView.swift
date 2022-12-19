//
//  SearchBuildingsView.swift
//  Campus
//
//  Created by user224354 on 10/2/22.
//

import SwiftUI

struct SearchBuildingsView: View {
    @EnvironmentObject var manager: MapManager
    var body: some View {
        Menu {
            ForEach($manager.buildings) { $building in
                BuildingSelectedView(building: $building)
            }
        } label: {
            Image(systemName: "magnifyingglass")
        }
    }
}

struct SearchBuildingsView_Previews: PreviewProvider {
    static var previews: some View {
        SearchBuildingsView()
    }
}
