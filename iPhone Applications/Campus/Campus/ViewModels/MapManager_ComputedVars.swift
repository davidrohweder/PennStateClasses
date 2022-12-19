//
//  MapManager_ComputedVars.swift
//  Campus
//
//  Created by user224354 on 10/2/22.
//

import Foundation

//MARK: Computed Variables
extension MapManager {
    
    var selectedBuilding: Int {
        guard let index = buildings.firstIndex(of: currentBuilding) else {return 0}
        print(index)
        return index
    }
    
    var selectedBuildings: [BuildingModel] {
        buildings.filter { $0.isSelected == true }
    }
    
}
