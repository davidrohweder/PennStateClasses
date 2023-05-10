//
//  MapCoordinator.swift
//  Campus
//
//  Created by user224354 on 10/17/22.
//

import Foundation
import MapKit

class MapCoordinator : NSObject, MKMapViewDelegate {
    let manager : MapManager
    let map:MKMapView
    
    init(_manager:MapManager, mapView:MKMapView) {
        manager = _manager
        map = mapView
    }
    
    func mapView(_ mapView: MKMapView, viewFor annotation: MKAnnotation) -> MKAnnotationView? {
        
        switch annotation {
        case is MKUserLocation: return nil
            
        case is BuildingModel:
            
            let marker = MKAnnotationView(annotation: annotation, reuseIdentifier: "")
            let building = annotation as! BuildingModel
            
            marker.canShowCallout = true
            marker.rightCalloutAccessoryView = UIButton(type: .detailDisclosure)
            
            let image = UIImage(systemName: "pin")!.withTintColor(.black)
            marker.image = image
            guard building.isFavorited != nil else {return marker} // check before we unwrap
            marker.image = UIImage(systemName: building.isFavorited! ? "circles.hexagonpath.fill" : "pin")!.withTintColor(building.isFavorited! ? .yellow : .black)
            return marker
        
        case is DroppedPin:
            let userMarker = MKAnnotationView(annotation: annotation, reuseIdentifier: "")
            userMarker.canShowCallout = true
            userMarker.rightCalloutAccessoryView = UIButton(type: .detailDisclosure)
            let image = UIImage(systemName: "mappin.and.ellipse")
            userMarker.image = image
            return userMarker
            
        default: return nil
            
        }
    }
    
    func mapView(_ mapView: MKMapView, annotationView view: MKAnnotationView, calloutAccessoryControlTapped control: UIControl) {
  
        switch view.annotation! {
        case is BuildingModel:
            let building = view.annotation as! BuildingModel
            manager.showConfirmation = true
            manager.currentBuilding = building
        default:
            let pin = view.annotation as! DroppedPin
            manager.currentPin = pin
            manager.pinConfirmation = true
        }
    }
    
    func mapView(_ mapView: MKMapView, rendererFor overlay: MKOverlay) -> MKOverlayRenderer {
        
        switch overlay {
        case is MKPolyline:
            let polyline = overlay as! MKPolyline
            let renderer = MKPolylineRenderer(polyline: polyline)
            renderer.strokeColor = UIColor.red
            renderer.lineWidth = 4
            return renderer
        case is MKCircle:
            let circle = overlay as! MKCircle
            let renderer = MKCircleRenderer(overlay: circle)
            return renderer
        case is MKPolygon:
            let polygon = overlay as! MKPolygon
            let renderer = MKPolygonRenderer(polygon: polygon)
            return renderer
        default:
            assert(false, "Unhandled Overlay")
        }
        
    }
    
    @objc func addPin(recognizer:UILongPressGestureRecognizer) {
        guard recognizer.state == .began else {return}
        let view = recognizer.view as! MKMapView
        let point = recognizer.location(in: view)
        let coordinate = view.convert(point, toCoordinateFrom: view)
        
        let pin = DroppedPin(coord: coordinate)
        manager.custannotations.append(pin)
        view.addAnnotation(pin)
        
    }
}
