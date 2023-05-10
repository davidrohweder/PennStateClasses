//
//  AppHeaderView.swift
//  BeeQ
//
//  Created by user224354 on 8/25/22.
//

import SwiftUI

struct AppHeaderView: View {
    var body: some View {
        VStack (alignment: .leading){
            Text("BeeQ").bold()
                .font(.title)
        }
    }
}

struct AppHeaderView_Previews: PreviewProvider {
    static var previews: some View {
        AppHeaderView()
    }
}
