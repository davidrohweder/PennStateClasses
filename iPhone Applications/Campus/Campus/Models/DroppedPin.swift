//
//  DroppedPin.swift
//  Around Town
//
//  Created by Hannan, John Joseph on 10/13/22.
//

import Foundation
import MapKit

class DroppedPin : NSObject, MKAnnotation {
    let coordinate: CLLocationCoordinate2D
    let title : String? 
    
    var placemark: MKPlacemark {
        return MKPlacemark(coordinate: coordinate)
    }
    
    init(coord:CLLocationCoordinate2D) {
        coordinate = coord
        title = "Custom Annotation"
    }
}
