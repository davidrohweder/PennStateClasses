//
//  BuildingDetailsView.swift
//  Campus
//
//  Created by user224354 on 10/2/22.
//

import SwiftUI

struct BuildingDetailsView: View {
    @EnvironmentObject var manager: MapManager
    var buildingIndex: Int
    var body: some View {
        VStack {
            Spacer()
            Image(systemName: manager.buildings[buildingIndex].isFavorited! ? "star.fill" : "star.slash")
                .foregroundColor( manager.buildings[buildingIndex].isFavorited! ? .orange : .blue)
            BuildingFavoritedView(building: $manager.buildings[buildingIndex])
            Text(manager.buildings[buildingIndex].name)
                .font(.system(size:30))
            Spacer()
            ImageOptionalView(buidlingIndex: buildingIndex)
            DismissSheetView()
        }
    }
}

struct BuildingDetailsView_Previews: PreviewProvider {
    static var previews: some View {
        BuildingDetailsView(buildingIndex: 0)
    }
}

