//
//  MainBackGroundView.swift
//  BeeQ
//
//  Created by user224354 on 8/25/22.
//

import SwiftUI

struct MainBackGroundView: View {
    var body: some View {
        Rectangle().fill(Color.orange).ignoresSafeArea()
    }
}

struct MainBackGroundView_Previews: PreviewProvider {
    static var previews: some View {
        MainBackGroundView()
    }
}
