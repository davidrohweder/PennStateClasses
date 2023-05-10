//
//  ClearRouteView.swift
//  Campus
//
//  Created by user224354 on 10/17/22.
//

import SwiftUI

struct ClearRouteView: View {
    @EnvironmentObject var manager: MapManager
    var body: some View {
        Button (action: {
            manager.clearExistingRoute()
        }) {
            Image(systemName: "location.slash.fill")
        }
        .disabled(manager.showDirections)
    }
}

struct ClearRouteView_Previews: PreviewProvider {
    static var previews: some View {
        ClearRouteView()
    }
}
