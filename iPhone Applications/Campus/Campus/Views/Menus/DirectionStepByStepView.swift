//
//  DirectionStepByStepView.swift
//  Campus
//
//  Created by user224354 on 10/17/22.
//

import SwiftUI

struct DirectionStepByStepView: View {
    @EnvironmentObject var manager: MapManager
    var body: some View {
        ScrollView (.vertical) {
            VStack {
                Text("Step by Step Instructions").bold()
                Spacer(minLength: 50.0)
                HStack {
                    Image(systemName: "location.north.circle")
                        .rotationEffect(.degrees(0 - manager.bearingAngle))
                    Text("Travel time to destination: \(manager.travelTime)")
                }
                Text("Miles: \(manager.travelDistance, specifier: "%.2f")")
                Divider()
                ForEach (manager.steps, id: \.self) { step in
                    VStack {
                        Text("\(step.instructions)")
                        Text("Miles: \(step.distance / 1609.344, specifier: "%.2f")")
                        Divider()
                    }
                }
            }
            DismissSheetView()
        }
    }
}

struct DirectionStepByStepView_Previews: PreviewProvider {
    static var previews: some View {
        DirectionStepByStepView()
    }
}
