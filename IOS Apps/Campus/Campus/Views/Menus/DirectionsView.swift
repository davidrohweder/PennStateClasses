//
//  DirectionsView.swift
//  Campus
//
//  Created by user224354 on 10/11/22.
//

import SwiftUI

struct DirectionsView: View {
    @EnvironmentObject var manager: MapManager
    var body: some View {
        VStack {
            HStack {
                Image(systemName: "location.north.circle")
                    .rotationEffect(.degrees(0 - manager.bearingAngle))
                if manager.isTraveling {
                    Text("Travel time to destination: \(manager.travelTime)")
                    Button (action: {
                        manager.showDirections.toggle()
                    }) {
                        Image(systemName: "map.circle")
                    }
                }
            }
            HStack {
                if manager.isTraveling {
                    Text("Miles: \(manager.travelDistance, specifier: "%.2f")")
                }
            }
        }
    }
}

struct DirectionsView_Previews: PreviewProvider {
    static var previews: some View {
        DirectionsView()
    }
}
