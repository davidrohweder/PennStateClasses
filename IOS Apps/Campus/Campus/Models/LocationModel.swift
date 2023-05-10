//
//  LocationModel.swift
//  Campus
//
//  Created by user224354 on 10/2/22.
//

import Foundation
import MapKit

struct LocationModel {
    
    let centerCoord = CLLocationCoordinate2D(latitude: 40.7964685139719, longitude: -77.8628317618596)
    let span = 0.005
    
    static let userSpan = MKCoordinateSpan(latitudeDelta: 0.005, longitudeDelta: 0.005)
}
