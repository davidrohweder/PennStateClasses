//
//  MainCampusView.swift
//  Campus
//
//  Created by user224354 on 10/2/22.
//

import SwiftUI

struct MainCampusView: View {
    @EnvironmentObject var manager : MapManager
    var body: some View {
        
        let buildingToolbarItem = ToolbarItem(placement: .navigationBarTrailing) {
            SearchBuildingsView()
        }

        let terrainSelector = ToolbarItem(placement: .navigationBarTrailing) {
            MapTerrainView()
        }
        
        let centerLocation = ToolbarItem(placement: .navigationBarTrailing) {
            CenterLocationView()
        }
        
        let clearRoute = ToolbarItem(placement: .navigationBarTrailing) {
            ClearRouteView()
        }
        
        let clearAnnotations = ToolbarItem(placement: .navigationBarTrailing) {
            RemoveCustomPins()
        }
        
        return NavigationStack {
            UIMap(manager: manager)
                .toolbar {
                    centerLocation
                    clearRoute
                    clearAnnotations
                    terrainSelector
                    buildingToolbarItem
                    ToolbarItem(placement: .bottomBar) {
                        DirectionsView()
                    }
                }
                .sheet(isPresented: $manager.showSheet, content: {
                    BuildingDetailsView(buildingIndex: manager.selectedBuilding)
                })
                .sheet(isPresented: $manager.showDirections, content: {
                    DirectionStepByStepView()
                })
            
                .confirmationDialog("spot", isPresented: $manager.showConfirmation, presenting: manager.currentBuilding) { building in
                    Button("Details") {
                        manager.currentBuilding = building
                        manager.showSheet = true
                    }
                    Button("Directions") {
                        manager.directionsFor(to: manager.currentBuilding.placemark)
                    }
                } message: { building in
                    Text("\(building.name)")
                }
                .confirmationDialog("spot", isPresented: $manager.pinConfirmation, presenting: manager.currentPin) { pin in
                    Button("Remove Pin") {
                        manager.custannotations.removeAll(where: { $0 == manager.currentPin })
                    }
                    Button("Directions") {
                        manager.directionsFor(to: manager.currentPin.placemark)
                    }
                } message: { building in
                    Text("\(manager.currentPin.title!)")
                }
        }
    }
}

struct MainCampusView_Previews: PreviewProvider {
    static var previews: some View {
        MainCampusView()
    }
}
