//
//  UIMap.swift
//  Campus
//
//  Created by user224354 on 10/17/22.
//

import SwiftUI
import MapKit


struct UIMap: UIViewRepresentable {
    @ObservedObject var manager : MapManager
    let mapView = MKMapView()
    
    func makeUIView(context: Context) -> MKMapView {

        mapView.mapType = .standard
        mapView.showsUserLocation = true
        mapView.userTrackingMode = manager.userTrackingMode
        mapView.delegate = context.coordinator
        
        let longPress = UILongPressGestureRecognizer(target: context.coordinator, action: #selector(context.coordinator.addPin(recognizer:)))
        mapView.addGestureRecognizer(longPress)
        
        return mapView
    }
    
    func updateUIView(_ mapView: MKMapView, context: Context) {
        mapView.setRegion(manager.region, animated: true)
        mapView.mapType = manager.terrainType == TerrainTypes.standard ? MKMapType.standard : (manager.terrainType == TerrainTypes.hybrid ? MKMapType.hybrid : MKMapType.satellite)
        
        let allAnnotations = mapView.annotations.filter { !($0 is MKUserLocation) }
        mapView.removeAnnotations(allAnnotations)
        
        mapView.addAnnotations(manager.selectedBuildings)
        mapView.addAnnotations(manager.custannotations)
        
        mapView.removeOverlays(mapView.overlays)
        mapView.addOverlays(manager.routes)
    }
    
    func makeCoordinator() -> MapCoordinator {
        return MapCoordinator(_manager: manager, mapView:mapView)
    }

    
}

struct UIMap_Previews: PreviewProvider {
    static var previews: some View {
        UIMap(manager: MapManager())
    }
}
