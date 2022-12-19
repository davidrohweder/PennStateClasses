//
//  CenterLocationView.swift
//  Campus
//
//  Created by user224354 on 10/11/22.
//

import SwiftUI
import MapKit

struct CenterLocationView: View {
    @EnvironmentObject var manager: MapManager
    var body: some View {
        Button (action: {
            guard manager.usersRecentLocation != nil else {return}
            manager.trackingUserLoc.toggle()
            manager.userTrackingMode = manager.trackingUserLoc ? .follow : .none
            if manager.userTrackingMode == .follow {
                manager.region.center = manager.usersRecentLocation!.coordinate
            }
        }) {
           Image(systemName: "location.fill.viewfinder")
        }
    }
}

struct CenterLocationView_Previews: PreviewProvider {
    static var previews: some View {
        CenterLocationView()
    }
}
