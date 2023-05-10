//
//  BuildingModel.swift
//  Campus
//
//  Created by user224354 on 10/2/22.
//

import Foundation
import MapKit

class BuildingModel : NSObject, Codable, Identifiable, MKAnnotation {
    
    // From JSON reader
    var latitude : Double
    var longitude : Double
    var name: String
    var opp_bldg_code: Int
    // Optionals From JSON reader
    var year_constructed: Int?
    var photo: String?
    
    // To be modified in view model, after init it will be apart of json when persisted
    var isFavorited: Bool?
    var isSelected: Bool?
    
    // Annotation - Computed property
    var coordinate: CLLocationCoordinate2D {
        CLLocationCoordinate2D(latitude: latitude, longitude: longitude)
    }
    
    // Placemark - Computer property
    var placemark: MKPlacemark {
        return MKPlacemark(coordinate: coordinate)
    }
    
    // title - MKAnno Req
    var title: String? // dup just for annotation
    
    // Identifible
    var id: Int {
        opp_bldg_code
    }
    
    init(lat: Double, long: Double, bName: String, bCode: Int, year: Int, photoPath: String, favorited: Bool, selected: Bool) {
        title = bName
        latitude = lat
        longitude = long
        name = bName
        opp_bldg_code = bCode
        year_constructed = year
        photo = photoPath
        isFavorited = favorited
        isSelected = selected
    }
    
    // default current building
    static let standard = BuildingModel(lat: 0.0, long: 0.0, bName: "", bCode: 0, year: 0, photoPath: "", favorited: false, selected: false)
}

