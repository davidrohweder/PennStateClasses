//
//  MapManager.swift
//  Campus
//
//  Created by David Rohweder.
//

import Foundation
import MapKit
import SwiftUI

class MapManager: NSObject, ObservableObject {
    
    @Published var locationModel = LocationModel()
    @Published var region : MKCoordinateRegion
    @Published var currentBuilding: BuildingModel
    @Published var currentPin : DroppedPin = DroppedPin(coord: CLLocationCoordinate2D(latitude: 0.0, longitude: 0.0))
    @Published var buildings : [BuildingModel] {
        didSet {
            // save data!
            buildingStorageManager.save(modelData: buildings)
            print("saved data")
        }
    }
    @Published var custannotations = [DroppedPin]()
    
    @Published var userTrackingMode: MKUserTrackingMode = .follow
    @Published var trackingUserLoc: Bool = true
    
    @Published var bearingAngle: Double = 0.0
    @Published var terrainType: TerrainTypes = .standard
    @Published var travelTime: String = "nil"
    @Published var travelDistance: Double = 0.0
    @Published var isTraveling: Bool = false
    @Published var steps = [MKRoute.Step]()
    
    // presenters
    @Published var showConfirmation : Bool = false
    @Published var pinConfirmation: Bool = false
    @Published var showSheet : Bool = false
    @Published var showDirections: Bool = false
    
    // mantinance vars
    private let buildingStorageManager: StorageManager<[BuildingModel]>
    
    var usersRecentLocation : CLLocation?
    @Published var usersLocationDescription : String?
    @Published var routes = [MKPolyline]()
    
    let locationManager : CLLocationManager
    
    override init() {
        let _locationModel = LocationModel()
        region = MKCoordinateRegion(center: _locationModel.centerCoord, span: MKCoordinateSpan(latitudeDelta: _locationModel.span, longitudeDelta: _locationModel.span))
        self.locationModel = _locationModel
        
        // Read in JSON
        buildingStorageManager = StorageManager(name: "buildings")
        let _buildings = buildingStorageManager.modelData ?? []
        buildings = MapManager.generateBuildings(modelBuildings: _buildings.sorted { $0.name.lowercased() < $1.name.lowercased() })
                
        currentBuilding = BuildingModel(lat: 0, long: 0, bName: "", bCode: 0, year: 0, photoPath: "", favorited: false, selected: false)
        locationManager = CLLocationManager()
        
        super.init()
        
        locationManager.desiredAccuracy = .leastNonzeroMagnitude
        locationManager.delegate = self
    }
    
    static func generateBuildings(modelBuildings: [BuildingModel]) -> [BuildingModel] {
        var buildings: [BuildingModel] = []
        for building in modelBuildings {
            let latitude = building.latitude
            let longitude = building.longitude
            let bld_name = building.name
            let bld_opp = building.opp_bldg_code
            // guard optionals
            var year: Int {
                guard let local_Year = building.year_constructed else {return 0}
                return local_Year
            }
            var pPath: String {
                guard let local_pPath = building.photo else {return ""}
                return local_pPath
            }
            var favorited: Bool {
                guard let local_favorited = building.isFavorited else {return false}
                return local_favorited
            }
            var selected: Bool {
                guard let local_selected = building.isSelected else {return false}
                return local_selected
            }
            buildings.append(BuildingModel(lat: latitude, long: longitude, bName: bld_name, bCode: bld_opp, year: year, photoPath: pPath, favorited: favorited, selected: selected))
        }
        return buildings
    }
    
}
