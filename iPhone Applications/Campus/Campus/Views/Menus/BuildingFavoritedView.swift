//
//  BuildingFavoritedView.swift
//  Campus
//
//  Created by user224354 on 10/3/22.
//

import SwiftUI

struct BuildingFavoritedView: View {
    @EnvironmentObject var manager: MapManager
    @Binding var building: BuildingModel
    var body: some View {
            Button(action: {
                manager.toggleFavorited(toggledBuilding: building)
            }) {
                HStack {
                    Image(systemName: building.isFavorited! ? "checkmark.square" : "square")
                        .foregroundColor(building.isFavorited! ? .orange : .black)
                    Text(building.name)
                }
            }
    }
}

struct BuildingFavoritedView_Previews: PreviewProvider {
    static var previews: some View {
        BuildingFavoritedView(building: .constant(BuildingModel.standard))
    }
}
