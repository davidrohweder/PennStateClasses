//
//  BuildingSelectedView.swift
//  Campus
//
//  Created by user224354 on 10/2/22.
//

import SwiftUI

struct BuildingSelectedView: View {
    @EnvironmentObject var manager: MapManager
    @Binding var building: BuildingModel
    var body: some View {
            Button(action: {
                manager.toggleSelected(toggledBuilding: building)
            }) {
                HStack {
                    Image(systemName: building.isSelected! ? "checkmark.square" : "square")
                    Text(building.name)
                }
            }
    }
}

struct BuildingSelectedView_Previews: PreviewProvider {
    static var previews: some View {
        BuildingSelectedView(building: .constant(BuildingModel.standard))
    }
}
