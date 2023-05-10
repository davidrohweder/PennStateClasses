//
//  ImageOptionalView.swift
//  Campus
//
//  Created by user224354 on 10/2/22.
//

import SwiftUI

struct ImageOptionalView: View {
    @EnvironmentObject var manager: MapManager
    var buidlingIndex: Int
    var body: some View {
        Image(manager.imageGiven(buildingIndex: buidlingIndex))
    }
}

struct ImageOptionalView_Previews: PreviewProvider {
    static var previews: some View {
        ImageOptionalView(buidlingIndex: 0)
    }
}
