//
//  RemoveCustomPins.swift
//  Campus
//
//  Created by user224354 on 10/18/22.
//

import SwiftUI

struct RemoveCustomPins: View {
    @EnvironmentObject var manager: MapManager
    var body: some View {
        Button (action: {
            manager.clearCustomPins()
        }) {
            Image(systemName: "mappin.slash.circle")
        }
    }
}

struct RemoveCustomPins_Previews: PreviewProvider {
    static var previews: some View {
        RemoveCustomPins()
    }
}
